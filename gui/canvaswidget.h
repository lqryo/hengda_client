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

//#include "markwidget.h"

#include "base/fs.h"
#include "base/fastring.h"
#include "base/json.h"
#include "base/co.h"
#include "base/rpc.h"

#include "base/time.h"

extern int g_port;
extern std::string g_serverip;
extern std::string g_cameraip;

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
		go(&CanvasWidget::client_fun, this);

//		int port = g_port;
//		qDebug() << "in canvaswidget, port is " << port;

		
	}

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *) override;

private:
	void client_fun()
	{
		auto borders = BordersLoader::Instance().save(undo_stacks_);
		std::cout << borders.dump(4) << std::endl;
		std::string info_str;


		for (const auto& border : borders) {
			auto key = border["ip"].get<std::string>();
			std::cout << "camera ip is " << key << std::endl;

			if (key == g_cameraip)
			{
				auto info = border["info"];
				info_str = info.dump();
				std::cout << info_str << std::endl;
			}
		}

		Json req, res;

		req.add_member("req_id", now::ms());
		req.add_member("method", "set_border_info");

		Json params = json::object();
		{
			params.add_member("ip", g_cameraip.c_str());
			params.add_member("port", 9910);
			Json info = json::array();
			info.parse_from(info_str.c_str());
			params.add_member("info", info);

		}
		req.add_member("params", params);

//		rpc::Client* c = rpc::new_client("192.168.2.111", 9910, "");
		std::cout << "server ip is " << g_serverip << std::endl;
		rpc::Client* c = rpc::new_client(g_serverip.c_str(), 9910, "");
		std::cout << req.pretty() << std::endl;
		c->call(req, res);
		std::cout << res.pretty() << std::endl;


		delete c;
	}

    QPainter painter_;

    shared_ptr<PaintCommand> command_ = nullptr;

    std::string key_{};

    std::map<std::string, CommandStack> undo_stacks_;
    std::map<std::string, CommandStack> redo_stacks_;

    Graph graph_ = Graph::NONE;
    unsigned int status_ = NONE_GRAPH;

    QPixmap pixmap_;
    QSize parea_;

	nlohmann::json data;

};


#endif //BORDER_CANVAS_WIDGET_H
