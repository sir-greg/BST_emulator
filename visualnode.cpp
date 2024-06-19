#include "visualnode.h"

VisualNode::VisualNode()
{

}

VisualNode::VisualNode(std::pair<int64_t, int64_t> info, QPointF center, qreal nodeRadius, TreeModes TreeMode, QFont font)
    : _info(info),
      _center(center),
      _nodeRadius(nodeRadius),
      _TreeMode(TreeMode),
      _font(font)
{}

QRectF VisualNode::boundingRect() const
{
    return QRectF(center().x() - nodeRadius(), center().y() - nodeRadius(),
                  2 * nodeRadius(), 2 * nodeRadius());
}

void VisualNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen defaultPen = QPen(Qt::black, 3);
    painter->setPen(defaultPen);
    switch (TreeMode())
    {
    case TreeModes::AVL:
        painter->setFont(font());
        painter->drawEllipse(boundingRect());
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(info().first));
        break;
    case TreeModes::REDBLACK:
        painter->setFont(font());
        if (info().second)
        {
            painter->setPen(QPen(Qt::red, 5));
        }
        painter->drawEllipse(boundingRect());
        if (info().second)
        {
            painter->setPen(defaultPen);
        }
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(info().first));
        break;
    case TreeModes::SPLAY:
        painter->setFont(font());
        painter->drawEllipse(boundingRect());
        painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(info().first));
        break;
    case TreeModes::TREAP:
        QFont boldFont = font();
        boldFont.setBold(true);
        painter->setFont(boldFont);
        QRectF bRect = boundingRect();
        double dotColSpace = QFontMetrics(font()).horizontalAdvance(";");
        painter->drawEllipse(boundingRect());
        painter->setPen(QPen(Qt::red, 5));
        double proportion = (double)QString::number(info().first).size() /
                (QString::number(info().first).size() + QString::number(info().second).size() + 1);
        painter->drawText(QRectF(bRect.x(), bRect.y(), bRect.width() * proportion, bRect.height()),
                          Qt::AlignRight | Qt::AlignVCenter, QString::number(info().first));
        painter->setPen(defaultPen);
        painter->setFont(font());
        painter->drawText(QRectF(bRect.x() + bRect.width() * proportion, bRect.y(),
                                 dotColSpace, bRect.height()), Qt::AlignCenter, ";");
        painter->setPen(QPen(Qt::blue, 5));
        painter->drawText(QRectF(bRect.x() + bRect.width() * proportion + dotColSpace,
                                 bRect.y(), bRect.width() * (1 - proportion) - dotColSpace, bRect.height()),
                          Qt::AlignLeft | Qt::AlignVCenter, QString::number(info().second));
        break;
    }
}

TreeModes VisualNode::TreeMode() const
{
    return _TreeMode;
}

qreal VisualNode::nodeRadius() const
{
    return _nodeRadius;
}

QPointF VisualNode::center() const
{
    return _center;
}

std::pair<int64_t, int64_t> VisualNode::info() const
{
    return _info;
}

QFont VisualNode::font() const
{
    return _font;
}

void VisualNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit deleteValue(info().first);
}
