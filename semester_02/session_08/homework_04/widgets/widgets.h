/// widgets.h

#ifndef WIDGETS_N
#define WIDGETS_N

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qmainwindow.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class widgets : public QMainWindow
{
    Q_OBJECT

public:
    widgets(QWidget *parent = nullptr);
    ~widgets();

private:
    QGroupBox* make_group_box(QWidget* parent) const;
    QComboBox* make_combo_box(QWidget* parent) const;
    QWidget* make_main_widget(QWidget* parent);

    QPushButton* _push_button{};
    QProgressBar* _progress_bar{};
};

#endif // !WIDGETS_N