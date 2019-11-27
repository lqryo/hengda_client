#ifndef BORDER_COMMAND_H
#define BORDER_COMMAND_H

#include <QObject>
#include <QPen>
#include <utility>
#include <vector>
#include <memory>
#include "utils.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;

class PaintCommand
{
public:
    explicit PaintCommand(Graph type) : graph_(type) { }
    PaintCommand(Graph type, QPen  pen)
        : graph_(type), pen_(std::move(pen))
    { }
    PaintCommand(Graph type, QPen  pen,  QVector<QPoint>  points)
        : graph_(type), pen_(std::move(pen)), points_(std::move(points))
    { }

    inline void graph(Graph t) { graph_ = t; }
    inline Graph graph() const  { return graph_; }

    inline void pen(const QPen& pen) { pen_ = pen; }
    inline QPen pen() const { return pen_; }

    inline void points(const QVector<QPoint>& ps) { points_ = ps; }
    inline const QVector<QPoint>& points() const { return points_; }
    inline QVector<QPoint>& points() { return points_; }


    void execute(QPainter *);

    QRect rect();
    QSize size();

private:
    Graph graph_{ NONE };
    QPen pen_{};
    QVector<QPoint> points_;

    QRect rect_;
};

class CommandStack : public QObject
{
    Q_OBJECT

public:
    CommandStack() = default;

    CommandStack(const CommandStack& other)
    {
        stack_ = other.stack_;
    }

    CommandStack& operator=(const CommandStack& other)
    {
        stack_ = other.stack_;
        emit changed(stack_.size());
        emit emptied(!stack_.size());
        return *this;
    }

    inline void push(std::shared_ptr<PaintCommand> command)
    {
        if(stack_.empty())
            emit emptied(false);

        stack_.push_back(command);

        emit changed(stack_.size());
        emit pushed();
    }

    inline void pop()
    {
        stack_.pop_back();

        if(stack_.empty())
            emit emptied(true);

        emit changed(stack_.size());
        emit popped();
    }

    inline size_t size() const { return stack_.size(); }

    inline std::shared_ptr<PaintCommand> back() const { return stack_.back(); }

    inline vector<shared_ptr<PaintCommand>> commands() { return stack_; }
    inline vector<shared_ptr<PaintCommand>> commands() const { return stack_; }

    inline bool empty() const { return stack_.empty(); }

signals:
    void changed(size_t);
    void pushed();
    void popped();

    void emptied(bool);               // emit when stack changed to be empty or not.

public slots:
    inline void clear()
    {
        if(!stack_.empty()) {
            emit changed(0);
            emit emptied(true);
        }

        stack_.clear();
    }

private:
    vector<shared_ptr<PaintCommand>> stack_;
};


#endif // BORDER_COMMAND_H
