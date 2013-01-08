#ifndef SM_INFODOCKWIDGET_H
#define SM_INFODOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class SM_InfoDockWidget;
}

class SM_InfoDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SM_InfoDockWidget(QWidget *parent = 0);
    ~SM_InfoDockWidget();
    
private:
    Ui::SM_InfoDockWidget *ui;
};

#endif // SM_INFODOCKWIDGET_H
