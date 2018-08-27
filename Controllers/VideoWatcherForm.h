#ifndef VIDEOWATCHERFORM_H
#define VIDEOWATCHERFORM_H

#include <QWidget>

namespace Ui {
class VideoWatcherForm;
}

class VideoWatcherForm : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWatcherForm(QWidget *parent = 0);
    ~VideoWatcherForm();

private:
    Ui::VideoWatcherForm *ui;
};

#endif // VIDEOWATCHERFORM_H
