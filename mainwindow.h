#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QFont>
#include <vector>
#include <utility>
#include <QStackedWidget>
#include "TreeModes.h"
#include "visualnode.h"
#include "C:\Users\overw\CODE\CPP\handy_structs\balanced_bsts\BBST.cpp"
using namespace HandyDS::Trees::AVL;
using namespace HandyDS::Trees::Cartesian;
using namespace HandyDS::Trees::RedBlack;
using namespace HandyDS::Trees::Splay;
using namespace HandyDS::Trees;
using namespace HandyDS::Trees::Common;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void insert_value();

    void draw_tree(std::vector<info_struct>& info);

    void insert_random_values();

    void remove_element_by_value();

    void remove_element(int64_t value);

    void clear_tree_button();

    void setScale(int fontSize);

    TreeModes getCurrentTreeMode() const;
    void setCurrentTreeMode(TreeModes newCurrentTreeMode);

private:

    qreal WindowWidth = 1000;
    qreal WindowHeight = 1000;

    QFont font = QFont("Helvetica", 10);

    Ui::MainWindow *ui;
    TreeModes currentTreeMode;

    QGraphicsScene* BST_GraphicsScene;
    QGraphicsView* BST_GraphicsView;

    Balanced_BST<int64_t, avl_tree<int64_t>> AVL_tree;
    Balanced_BST<int64_t, RB_Tree<int64_t>> REDBLACK_tree;
    Balanced_BST<int64_t, SplayTree<int64_t>> SPLAY_tree;
    Balanced_BST<int64_t, Treap<int64_t>> TREAP_tree;
};
#endif // MAINWINDOW_H
