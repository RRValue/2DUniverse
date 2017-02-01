#include "2DUniverse/Controller/DefaultOptionWidget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

DefaultOptionWidget::DefaultOptionWidget()
{
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    QLabel* label = new QLabel(this);
    
    label->setLayoutDirection(Qt::LeftToRight);
    label->setAlignment(Qt::AlignCenter);
    label->setText(tr("No Options"));

    horizontalLayout->addWidget(label);
}

DefaultOptionWidget::~DefaultOptionWidget()
{

}
