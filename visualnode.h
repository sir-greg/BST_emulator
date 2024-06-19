#ifndef VISUALNODE_H
#define VISUALNODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QFont>
#include <QObject>
#include "TreeModes.h"

class VisualNode : public QObject, public QGraphicsItem
{

Q_OBJECT

public:
    VisualNode();

    VisualNode(std::pair<int64_t, int64_t> info, QPointF center, qreal nodeRadius, TreeModes TreeMode, QFont font);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    TreeModes TreeMode() const;

    qreal nodeRadius() const;

    QPointF center() const;

    std::pair<int64_t, int64_t> info() const;

    QFont font() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

signals:

    void deleteValue(int64_t value);

private:
    std::pair<int64_t, int64_t> _info;
    QPointF _center;
    qreal _nodeRadius;
    TreeModes _TreeMode;

    QFont _font;

};

#endif // VISUALNODE_H
