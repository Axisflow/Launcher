#ifndef THEATERVIEW_H
#define THEATERVIEW_H

#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QScrollArea>

#include "defaultTheaterView_global.h"
#include "struct.h"
#include "plugin/plugin.h"

class TheaterView;

class TermIndex {
    public:
        TermIndex();
        TermIndex(qint32 &index);
        qint32 index;
        static qint32 lastPressed;
};

class ItemLabel: public QLabel, public TermIndex {
    Q_OBJECT
    public:
        ItemLabel(QWidget *parent = nullptr);
        ItemLabel(QWidget *parent, qint32 &index);

        void copy(QLabel *);

    // QWidget interface
    protected:
        void wheelEvent(QWheelEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
};

class WheelArea: public QWidget {
    Q_OBJECT
    public:
        WheelArea(QWidget *parent = nullptr);

        // QWidget interface
    protected:
        void wheelEvent(QWheelEvent *event) override;
};

class ItemContainer: public QFrame, public TermIndex {
    Q_OBJECT
    public:
        ItemContainer();
        QVBoxLayout *layout;
        ItemLabel *name;
        ItemLabel *icon;
        QPixmap *icon_data;

        void setIndex(qint32 index);
        static void resizeFont(QWidget *, qint32);

    // QWidget interface
    protected:
        void wheelEvent(QWheelEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

    private slots:
        void resizeChildren();
};

class DEFAULTTHEATERVIEW_EXPORT TheaterView: public SceneryPlugin {
    Q_OBJECT
    public:
        inline static TheaterView *plugin;
        TheaterView(QMainWindow *, QVector<ItemData> *);

        QMainWindow *window;
        QVector<ItemData> *items_data;

        QWidget *main_layout_widget;
        QVBoxLayout *main_layout;
        QFrame *top_frame;
        QHBoxLayout *middle_layout;
        QFrame *left_frame;
        QVBoxLayout *right_layout;
        QWidget *bottom_area_widget;

        qint32 nowIndex;
        QLabel *title;
        QLabel *description;
        QLabel *poster;
        QMap<qint32, QPixmap> poster_data;
        void posterResize();

        QWidget *group_container;
        QHBoxLayout *items_group;

        qint32 item_size;
        ItemContainer *item_containers;

        void resize();
        void build();
        void changeItem(qint32 index);

    signals:
        void sceneryResized();

    public slots:
        void resizeEvent(QResizeEvent *&) override;
};

#endif // THEATERVIEW_H
