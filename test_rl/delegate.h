#pragma once
#include <QStyledItemDelegate>
#include <QLineEdit>

class spinbox_double : public QStyledItemDelegate
{
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    //编辑的时候设置数据到上面创建的editor中。
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //编辑完成，保存数据到data中
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //设置编辑控件的位置和大小、样式等
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
public:
    spinbox_double(const double& min, const double& max, const unsigned int& decimals = 3, const QString& prefix = "", const QString& delimiter="");
private:
    const double _min, _max;
    const unsigned int _decimals;
    QString _prefix, _delimiter;
};



class spinbox_int : public QStyledItemDelegate
{
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    //编辑的时候设置数据到上面创建的editor中。
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //编辑完成，保存数据到data中
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //设置编辑控件的位置和大小、样式等
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
public:
    spinbox_int(const int& min, const int& max);
private:
    const int _min, _max;
};

class text : public QStyledItemDelegate
{
private:
    QLineEdit::EchoMode _type;
    bool _enabled;
public:
    text(const bool enabled = true, QLineEdit::EchoMode type = QLineEdit::Normal);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    //编辑的时候设置数据到上面创建的editor中。
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //编辑完成，保存数据到data中
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //设置编辑控件的位置和大小、样式等
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* painter,
        const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  
};

class combobox : public QStyledItemDelegate
{
private:
    QStringList _data;
    QMap<QString, int> _index;
public:
    combobox(const QStringList& data);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    //编辑的时候设置数据到上面创建的editor中。
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //编辑完成，保存数据到data中
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //设置编辑控件的位置和大小、样式等
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
     

