// 绘制边界用的控件
// 绘制过程作为状态机

#ifndef BORDER_CANVAS_WIDGET_H
#define BORDER_CANVAS_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <utility>
#include "command.h"
#include "bordersloader.h"
#include "json.hpp"
#include "tcpclient.h"
//#include "markwidget.h"

extern int g_port;

#define DEFINE_PAINTING_STATES(X) START_PAINTING_##X    = START_PAINTING | X,   PAINTING_##X    = PAINTING | X

class CanvasWidget : public QWidget {
    Q_OBJECT

    enum EditStatus: unsigned int {
        NONE_GRAPH, START_PAINTING = 0x0001'0000, PAINTING = 0x0002'0000,
        DEFINE_PAINTING_STATES(RECTANGLE),
        DEFINE_PAINTING_STATES(ELLIPSE),
        DEFINE_PAINTING_STATES(LINE),
        DEFINE_PAINTING_STATES(CURVES),
        DEFINE_PAINTING_STATES(POLYGON),
        DEFINE_PAINTING_STATES(BROKEN_LINE),
        GRAPH_MASK = 0x0000'ffff
    };

public:
    explicit CanvasWidget(QWidget* parent = nullptr)
        : QWidget(parent) {
        setMouseTracking(true);

        load();
        bind();
    }

    void setPixmap(QPixmap pixmap) 
	{ 
		pixmap_ = std::move(pixmap);
		parea_ = pixmap_.scaled(rect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation).size();
		update();
	}

    void load() { undo_stacks_ = BordersLoader::Instance().load(); }

    // 从文件中加载标记数据后，为每个视频流数据绑定undo/redo操作
    void bind()
    {
        for(const auto& stack : undo_stacks_) {
            connect(&stack.second, &CommandStack::emptied, this, &CanvasWidget::undoEmptied);


            if(redo_stacks_.find(stack.first) == redo_stacks_.end()) {
                redo_stacks_[stack.first] = CommandStack();
            }
            connect(&redo_stacks_[stack.first], &CommandStack::emptied, this, &CanvasWidget::redoEmptied);
        }
    }

    void setKey(const std::string &key)
    {
        key_ = key;
        if(undo_stacks_.find(key_) == undo_stacks_.end()) {
            undo_stacks_[key_] = CommandStack();
            redo_stacks_[key_] = CommandStack();

            connect(&undo_stacks_[key_], &CommandStack::emptied, this, &CanvasWidget::undoEmptied);
            connect(&redo_stacks_[key_], &CommandStack::emptied, this, &CanvasWidget::redoEmptied);
        }

        // 初始状态可能不为空
        undoEmptied(undo_stacks_[key_].empty());
    }

signals:
    void undoEmptied(bool);
    void redoEmptied(bool);

public slots:
    void graph(Graph graph)
    {
        graph_ = graph;
        status_ = START_PAINTING | graph_;
    }

	void undo();
	void redo();
	void clear();

    void save() 
	{ 
		auto str = BordersLoader::Instance().save(undo_stacks_);
		int port = g_port;
		qDebug() << "in canvaswidget, port is " << port;
		if(port == 0){
			TcpClient1::instance().response("/camera/borders", str);
		}
		else if (port == 1) {
			TcpClient2::instance().response("/camera/borders", str);
		}
		else if (port == 2) {
			TcpClient3::instance().response("/camera/borders", str);
		}
		else if (port == 3) {
			TcpClient4::instance().response("/camera/borders", str);
		}
		else if (port == 4) {
			TcpClient5::instance().response("/camera/borders", str);
		}
		else if (port == 5) {
			TcpClient6::instance().response("/camera/borders", str);
		}
		else if (port == 6) {
			TcpClient7::instance().response("/camera/borders", str);
		}
		else if (port == 7) {
			TcpClient8::instance().response("/camera/borders", str);
		}
		else if (port == 8) {
			TcpClient9::instance().response("/camera/borders", str);
		}
		else if (port == 9) {
			TcpClient10::instance().response("/camera/borders", str);
		}
		else if (port == 10) {
			TcpClient11::instance().response("/camera/borders", str);
		}
		else if (port == 11) {
			TcpClient12::instance().response("/camera/borders", str);
		}
		else if (port == 12) {
			TcpClient13::instance().response("/camera/borders", str);
		}
		else if (port == 13) {
			TcpClient14::instance().response("/camera/borders", str);
		}
		else if (port == 14) {
			TcpClient15::instance().response("/camera/borders", str);
		}
		
	}

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *) override;

private:
    QPainter painter_;

    shared_ptr<PaintCommand> command_ = nullptr;

    std::string key_{};

    std::map<std::string, CommandStack> undo_stacks_;
    std::map<std::string, CommandStack> redo_stacks_;

    Graph graph_ = Graph::NONE;
    unsigned int status_ = NONE_GRAPH;

    QPixmap pixmap_;
    QSize parea_;
};


#endif //BORDER_CANVAS_WIDGET_H
