#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
    // Test HESMersh
    {
        HESMesh mesh;
        mesh.addVertex();
        mesh.addVertex();
        mesh.addVertex();
        mesh.addVertex();
        mesh.addVertex();

        mesh.addTriangle(0, 1, 2);
        mesh.addTriangle(0, 2, 3);
        mesh.addTriangle(4, 3, 2);

        HESBuilder builder(&mesh);
        builder.build();
    }

    QApplication app(argc, argv);
    
    HalfEdge2DWidget* bw = new HalfEdge2DWidget();
    bw->show();

    return app.exec();
}
