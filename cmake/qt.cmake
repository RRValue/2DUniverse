FUNCTION(GATHERMOCHEADER MOCFILES)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs HEADER)

    cmake_parse_arguments(GATHERMOCHEADER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # test if get a proper output from moc
    SET(MOCOUTPUTFILES)
    FOREACH(HEADERFILE ${GATHERMOCHEADER_HEADER})
        GET_FILENAME_COMPONENT(MOCWORKONGDIR ${HEADERFILE} DIRECTORY)
        EXECUTE_PROCESS(COMMAND ${QT_MOC_EXECUTABLE} ${HEADERFILE}
                WORKING_DIRECTORY ${MOCWORKONGDIR}
                ERROR_VARIABLE MOCERROR OUTPUT_QUIET)

        IF("${MOCERROR}" STREQUAL "")
            STRING(REPLACE ${PrjSrcDir} ${PrjMocDir} MOCOUTPUTFILE ${HEADERFILE})
            
            GET_FILENAME_COMPONENT(MOCFILE_DIRECTORY ${MOCOUTPUTFILE} DIRECTORY)
            GET_FILENAME_COMPONENT(MOCFILE_NAME_WE   ${MOCOUTPUTFILE} NAME_WE)
                
            SET(MOCOUTPUTFILE ${MOCFILE_DIRECTORY}/${MOCFILE_NAME_WE}_moc.cpp)
                
            # generate moc command for each header
            QT5_GENERATE_MOC(${HEADERFILE} ${MOCOUTPUTFILE})
                
            SET(MOCOUTPUTFILES ${MOCOUTPUTFILES} ${MOCOUTPUTFILE})
        ENDIF("${MOCERROR}" STREQUAL "")
    ENDFOREACH(HEADERFILE ${GATHERMOCHEADER_HEADER})
    
    # set Moc files
    SET(${MOCFILES} ${MOCOUTPUTFILES} PARENT_SCOPE)
ENDFUNCTION(GATHERMOCHEADER MOCFILES)

FUNCTION(GATHERUIFILES UIC_FILES) 
    set(options)
    set(oneValueArgs)
    set(multiValueArgs UIFILES)

    cmake_parse_arguments(GATHERUIFILES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    SET(UI_OPTIONS)
    
    # process uic files
    SET(UICOUTPUTFILES)
    FOREACH(UIFILE ${GATHERUIFILES_UIFILES})
        STRING(REPLACE ${PrjSrcDir} ${PrjUicDir} UICOUTPUTFILE ${UIFILE})
            
        GET_FILENAME_COMPONENT(UICFILE_DIRECTORY ${UICOUTPUTFILE} DIRECTORY)
        GET_FILENAME_COMPONENT(UICFILE_NAME_WE   ${UICOUTPUTFILE} NAME_WE)
            
        SET(UICOUTPUTFILE ${UICFILE_DIRECTORY}/${UICFILE_NAME_WE}_uic.h)
            
        # generate moc command for each header
        add_custom_command(OUTPUT ${UICOUTPUTFILE}
          COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
          ARGS ${UI_OPTIONS} -o ${UICOUTPUTFILE} ${UIFILE}
          MAIN_DEPENDENCY ${UIFILE} VERBATIM)
            
        SET(UICOUTPUTFILES ${UICOUTPUTFILES} ${UICOUTPUTFILE})
    ENDFOREACH(UIFILE ${GATHERUIFILES_UIFILES})
    
    # set Uic files
    SET(${UIC_FILES} ${UICOUTPUTFILES} PARENT_SCOPE)
ENDFUNCTION(GATHERUIFILES UIC_FILES)

FUNCTION(GATHERQRCFILES QRC_OUT_FILES) 
    set(options)
    set(oneValueArgs)
    set(multiValueArgs QRCFILES)

    cmake_parse_arguments(GATHERQRCFILES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # store current cmake binary dir
    SET(TMP_CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
    SET(CMAKE_CURRENT_BINARY_DIR ${PrjQrcDir})
    
    SET(QRCOUTPUTFILES)
    QT5_ADD_RESOURCES(QRCOUTPUTFILES ${GATHERQRCFILES_QRCFILES})
    
    # restore cmake binary dir
    SET(CMAKE_CURRENT_BINARY_DIR ${TMP_CMAKE_CURRENT_BINARY_DIR})
    
    # set Uic files
    SET(${QRC_OUT_FILES} ${QRCOUTPUTFILES} PARENT_SCOPE)
ENDFUNCTION(GATHERQRCFILES QRC_OUT_FILES)

FUNCTION(COLLECTQTDEPBINS dbg_bins rel_bins)
    SET(TMP_REL_BIN)
    SET(TMP_DBG_BIN)
    
    FOREACH(QTPACKAGE ${QTPACKAGELIST})
        GET_TARGET_PROPERTY(QT5_DLL_PATH_REL Qt5::${QTPACKAGE} LOCATION_Release)
        GET_TARGET_PROPERTY(QT5_DLL_PATH_DBG Qt5::${QTPACKAGE} LOCATION_Debug)
        
        LIST(APPEND TMP_REL_BIN ${QT5_DLL_PATH_REL})
        LIST(APPEND TMP_DBG_BIN ${QT5_DLL_PATH_DBG})
    ENDFOREACH(QTPACKAGE ${QTPACKAGELIST})
    
    SET(${dbg_bins} ${${dbg_bins}} ${TMP_DBG_BIN} PARENT_SCOPE)
    SET(${rel_bins} ${${rel_bins}} ${TMP_REL_BIN} PARENT_SCOPE)
ENDFUNCTION(COLLECTQTDEPBINS dbg_bins rel_bins)

FUNCTION(COLLECTQTPLUGINS dbg_plugins rel_plugins)
    SET(TMP_REL_PLUGINS)
    SET(TMP_DBG_PLUGINS)
    
    # collect plugins
    SET(QTPLUGINS)
    FOREACH(QTPACKAGE ${QTPACKAGELIST})
        FOREACH(QT_PLUGIN ${Qt5${QTPACKAGE}_PLUGINS})
            LIST(APPEND QTPLUGINS ${QT_PLUGIN})
        ENDFOREACH(QT_PLUGIN ${Qt5${QTPACKAGE}_PLUGINS})
    ENDFOREACH(QTPACKAGE ${QTPACKAGELIST})
    
    LIST(REMOVE_DUPLICATES QTPLUGINS)
    
    FOREACH(QTPLUGIN ${QTPLUGINS})
        GET_TARGET_PROPERTY(QT5_PLUGIN_DLL_PATH_REL ${QTPLUGIN} LOCATION_Release)
        GET_TARGET_PROPERTY(QT5_PLUGIN_DLL_PATH_DBG ${QTPLUGIN} LOCATION_Debug)
        
        LIST(APPEND TMP_REL_PLUGINS ${QT5_PLUGIN_DLL_PATH_REL})
        LIST(APPEND TMP_DBG_PLUGINS ${QT5_PLUGIN_DLL_PATH_DBG})
    ENDFOREACH(QTPLUGIN ${QTPLUGINS})
    
    SET(${dbg_plugins} ${${dbg_plugins}} ${TMP_DBG_PLUGINS} PARENT_SCOPE)
    SET(${rel_plugins} ${${rel_plugins}} ${TMP_REL_PLUGINS} PARENT_SCOPE)
ENDFUNCTION(COLLECTQTPLUGINS dbg_plugins rel_plugins)

FUNCTION(COPYQTPLUGINS)
    set(options)
    set(oneValueArgs REL_PATH DBG_PATH)
    set(multiValueArgs REL_PLUGINS DBG_PLUGINS)

    cmake_parse_arguments(CPYPL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
        
    FOREACH(Rel_Plugin ${CPYPL_REL_PLUGINS})
        FILE(RELATIVE_PATH RELATIVE_PLUGIN_PATH  ${QT5_PATH}/plugins ${Rel_Plugin})
        CONFIGURE_FILE(${Rel_Plugin} ${CPYPL_REL_PATH}/${RELATIVE_PLUGIN_PATH} COPYONLY)
    ENDFOREACH(Rel_Plugin ${CPYPL_REL_PLUGINS})
    
    FOREACH(Dbg_Plugin ${CPYPL_DBG_PLUGINS})
        FILE(RELATIVE_PATH RELATIVE_PLUGIN_PATH  ${QT5_PATH}/plugins ${Dbg_Plugin})
        CONFIGURE_FILE(${Dbg_Plugin} ${CPYPL_DBG_PATH}/${RELATIVE_PLUGIN_PATH} COPYONLY)
    ENDFOREACH(Dbg_Plugin ${CPYPL_DBG_PLUGINS})
ENDFUNCTION(COPYQTPLUGINS)