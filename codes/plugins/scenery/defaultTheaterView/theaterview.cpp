#include "theaterview.h"

extern "C" DEFAULTTHEATERVIEW_EXPORT SceneryPlugin *init(QMainWindow *launcher, QVector<ItemData> *items) {
    SceneryPlugin *scenery = new TheaterView(launcher, items);
    return scenery;
}

TheaterView::TheaterView(QMainWindow *parent, QVector<ItemData> *items_data) {
    this->window = parent;
    this->items_data = items_data;
    this->poster = nullptr;

    parent->setMinimumSize(600, 450);

    QPalette p(parent->centralWidget()->palette());
    p.setColor(QPalette::ColorRole::Window, QColor(16, 16, 16, 160));
    p.setColor(QPalette::ColorRole::WindowText, QColor(255, 255, 255));
    parent->centralWidget()->setPalette(p);

    TheaterView::plugin = this;

    this->build();
    this->nowIndex = 0;
    this->changeItem(0);
}

void TheaterView::build() {
    this->main_layout_widget = new QWidget(this->window->centralWidget());
    this->main_layout_widget->setObjectName("main_layout_widget");
    this->main_layout = new QVBoxLayout(this->main_layout_widget);
    this->main_layout->setSpacing(0);
    this->main_layout->setObjectName("main_layout");
    this->main_layout->setContentsMargins(0, 0, 0, 0);
    top_frame = new QFrame(this->main_layout_widget);
    top_frame->setObjectName("top_frame");
    top_frame->setFrameShape(QFrame::StyledPanel);
    top_frame->setFrameShadow(QFrame::Raised);

    main_layout->addWidget(top_frame);

    middle_layout = new QHBoxLayout();
    middle_layout->setSpacing(2);
    middle_layout->setObjectName("middle_layout");
    left_frame = new QFrame(this->main_layout_widget);
    left_frame->setObjectName("left_frame");
    left_frame->setFrameShape(QFrame::StyledPanel);
    left_frame->setFrameShadow(QFrame::Raised);

    middle_layout->addWidget(left_frame);

    right_layout = new QVBoxLayout();
    right_layout->setObjectName("right_layout");

    this->title = new QLabel(this->main_layout_widget);
    this->title->setObjectName("title");
    this->title->setAlignment(Qt::AlignmentFlag::AlignBottom | Qt::AlignmentFlag::AlignLeft);

    right_layout->addWidget(title);

    this->description = new QLabel(this->main_layout_widget);
    this->description->setObjectName("description");
    this->description->setAlignment(Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignLeft);

    right_layout->addWidget(description);

    right_layout->setStretch(0, 1);
    right_layout->setStretch(1, 2);

    middle_layout->addLayout(right_layout);

    middle_layout->setStretch(0, 1);
    middle_layout->setStretch(1, 2);

    main_layout->addLayout(middle_layout);

    this->bottom_area_widget = new QWidget(this->main_layout_widget);
    this->bottom_area_widget->setObjectName("bottom_area_widget");

    main_layout->addWidget(bottom_area_widget);

    main_layout->setStretch(0, 5);
    main_layout->setStretch(1, 3);
    main_layout->setStretch(2, 2);

    this->group_container = new WheelArea(this->bottom_area_widget);
    this->group_container->setObjectName("group_container");
    this->group_container->move(0, 0);
    this->items_group = new QHBoxLayout(this->group_container);
    this->items_group->setObjectName("items_group");
    this->items_group->setSpacing(0);
    this->items_group->setContentsMargins(0, 0, 0, 0);
    this->item_size = items_data->size();

    this->item_containers = new ItemContainer[this->item_size];
    for(qint32 i = 0; i < this->item_size; i++) {
        if((*items_data)[i].poster) {
            this->poster_data[i] = (*items_data)[i].poster->pixmap();
            (*items_data)[i].poster->setParent(this->left_frame);
            (*items_data)[i].poster->move(0, 0);
            (*items_data)[i].poster->setVisible(false);
        }

        this->item_containers[i].setParent(this->group_container);
        this->item_containers[i].setIndex(i);
        this->item_containers[i].name->setText((*items_data)[i].name);
        if((*items_data)[i].icon) {
            this->item_containers[i].icon->copy((*items_data)[i].icon);
            this->item_containers[i].icon_data = new QPixmap((*items_data)[i].icon->pixmap());
        }

        this->items_group->addWidget(&this->item_containers[i], 1);
    }

    ItemContainer::resizeFont(this->title, 30);
    ItemContainer::resizeFont(this->description, 18);
}

void TheaterView::resize() {
    qint32 w = this->window->centralWidget()->size().width();
    qint32 h = this->window->centralWidget()->size().height();
    this->main_layout_widget->setGeometry(QRect(0, 0, w, h));

    qint32 size = qMin(this->bottom_area_widget->size().height(), this->bottom_area_widget->size().width());
    this->group_container->resize(size * this->item_size, this->bottom_area_widget->size().height());
    this->group_container->move((this->group_container->size().width() > this->bottom_area_widget->size().width() ? 0 : (this->bottom_area_widget->size().width() - this->group_container->size().width()) / 2), 0);
    this->items_group->setContentsMargins(size * 0.1, size * 0.1, size * 0.1, size * 0.1);
    this->items_group->setSpacing(size * 0.2);
    this->posterResize();

    emit this->sceneryResized();
}

void TheaterView::posterResize() {
    if(this->poster) {
        this->poster->resize(this->left_frame->size());
        this->poster->setPixmap(this->poster_data[this->nowIndex].scaled(this->left_frame->size() * 0.9, Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
    }
}

void TheaterView::changeItem(qint32 index) {
    if(index >= 0 && index < this->item_size && this->item_size > 0) {
        this->item_containers[this->nowIndex].setStyleSheet(".ItemContainer { background-color: rgba(0, 0, 0, 0); border-radius: 10px; } QLabel { color: rgb(255, 255, 255); }");
        this->nowIndex = index;
        this->item_containers[this->nowIndex].setStyleSheet(".ItemContainer { background-color: rgba(16, 16, 16, 0.5); color: rgb(255, 255, 255); border-radius: 10px; } QLabel { color: rgb(255, 255, 255); }");
        this->title->setText((*TheaterView::plugin->items_data)[index].name);
        this->description->setText((*TheaterView::plugin->items_data)[index].description);

        if(this->poster) this->poster->setVisible(false);
        if((*this->items_data)[index].poster) {
            this->poster = (*this->items_data)[index].poster;
            this->posterResize();
            this->poster->setVisible(true);
        }

        qint32 size = qMin(this->bottom_area_widget->size().height(), this->bottom_area_widget->size().width()) * 0.1;
        qint32 disless = this->item_containers[this->nowIndex].mapTo(this->bottom_area_widget, QPoint(0, 0)).x();
        qint32 dismore = this->item_containers[this->nowIndex].mapTo(this->bottom_area_widget, QPoint(this->item_containers[this->nowIndex].size().width(), 0)).x();
        if(disless < 0) {
            this->group_container->move(this->group_container->pos().x() - disless + size ,0);
        } else if(dismore - this->bottom_area_widget->size().width() > 0) {
            this->group_container->move(this->group_container->pos().x() - dismore + this->bottom_area_widget->width() - size, 0);
        }
    }
}

void TheaterView::resizeEvent(QResizeEvent *&e) {
    this->resize();
}

qint32 TermIndex::lastPressed = -1;
TermIndex::TermIndex() { }
TermIndex::TermIndex(qint32 &index): index(index) { }

ItemContainer::ItemContainer() {
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->layout = new QVBoxLayout(this);
    this->icon = new ItemLabel(this);
    this->name = new ItemLabel(this);
    this->name->setAlignment(Qt::AlignmentFlag::AlignCenter);
    this->layout->addWidget(this->icon, 3);
    this->layout->addWidget(this->name, 2);
    this->resizeFont(this->name, 15);
    this->setStyleSheet("color: rgb(255, 255, 255);");

    connect(TheaterView::plugin, &TheaterView::sceneryResized, this, &ItemContainer::resizeChildren);
}

void ItemContainer::setIndex(qint32 index) {
    this->index = index;
    this->name->index = index;
    this->icon->index = index;
}

void ItemContainer::resizeFont(QWidget *w, qint32 size) {
    QFont f = w->font();
    f.setPixelSize(size);
    w->setFont(f);
}

void ItemContainer::wheelEvent(QWheelEvent *event) {
    if(event->angleDelta().x() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().x() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }

    if(event->angleDelta().y() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().y() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }
}

slots void ItemContainer::resizeChildren() {
    this->icon->setPixmap(this->icon_data->scaled(this->icon->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
}

void ItemContainer::mouseMoveEvent(QMouseEvent *event) {
    if(event->type()==QMouseEvent::Enter) {

    } else if(event->type()==QMouseEvent::Leave) {

    }
}

void ItemContainer::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::MouseButton::LeftButton) {
        if(this->lastPressed == this->index && !(event->flags() & Qt::MouseEventFlag::MouseEventCreatedDoubleClick)) emit TheaterView::plugin->Execute(this->index);
        else {
            TheaterView::plugin->changeItem(this->index);
        }

        this->lastPressed = this->index;
    }
}

void ItemContainer::mouseDoubleClickEvent(QMouseEvent *event) {
    if(event->button() == Qt::MouseButton::LeftButton) {
        TheaterView::plugin->changeItem(this->index);
        emit TheaterView::plugin->Execute(this->index);
        this->lastPressed = this->index;
    }
}

ItemLabel::ItemLabel(QWidget *parent): QLabel(parent) {}
ItemLabel::ItemLabel(QWidget *parent, qint32 &index): QLabel(parent), TermIndex(index) { }

void ItemLabel::copy(QLabel *l) {
    this->setPicture(l->picture());
    this->setPixmap(l->pixmap());
    this->setPalette(l->palette());
    this->setAlignment(l->alignment());
    this->setIndent(l->indent());
    this->setMargin(l->margin());
    this->setResourceProvider(l->resourceProvider());
    this->setText(l->text());
    this->setTextFormat(l->textFormat());
    this->setTextInteractionFlags(l->textInteractionFlags());
    this->setWordWrap(l->wordWrap());
    this->setFrameRect(l->frameRect());
    this->setFrameShadow(l->frameShadow());
    this->setFrameShape(l->frameShape());
    this->setFrameStyle(l->frameStyle());
    this->setLineWidth(l->lineWidth());
    this->setMidLineWidth(l->midLineWidth());
    this->setStyleSheet(l->styleSheet());
    if(l->movie()) this->setMovie(l->movie());
}

void ItemLabel::wheelEvent(QWheelEvent *event) {
    if(event->angleDelta().x() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().x() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }

    if(event->angleDelta().y() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().y() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }
}

void ItemLabel::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::MouseButton::LeftButton) {
        if(this->lastPressed == this->index && !(event->flags() & Qt::MouseEventFlag::MouseEventCreatedDoubleClick)) emit TheaterView::plugin->Execute(this->index);
        else {
            TheaterView::plugin->changeItem(this->index);
        }

        this->lastPressed = this->index;
    }
}

void ItemLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    if(event->button() == Qt::MouseButton::LeftButton) {
        TheaterView::plugin->changeItem(this->index);
        emit TheaterView::plugin->Execute(this->index);
        this->lastPressed = this->index;
    }
}

void ItemLabel::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key::Key_Enter || event->key() == Qt::Key::Key_Space) {
        if(this->lastPressed == this->index) emit TheaterView::plugin->Execute(this->index);
        else {
            TheaterView::plugin->changeItem(this->index);
        }

        this->lastPressed = this->index;
    }
}

WheelArea::WheelArea(QWidget *parent): QWidget(parent) { }

void WheelArea::wheelEvent(QWheelEvent *event) {
    if(event->angleDelta().x() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().x() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }

    if(event->angleDelta().y() > 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex - 1);
    } else if(event->angleDelta().y() < 0) {
        TheaterView::plugin->changeItem(TheaterView::plugin->nowIndex + 1);
    }
}
