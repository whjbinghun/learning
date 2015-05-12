#ifndef MATRIXLAYOUTFACTORY_H
#define MATRIXLAYOUTFACTORY_H

#include <QRect>
#include <QVector>
#include <QMap>
#include <QColor>


#define MS_WINDOW_WIDTH 640     // 标准窗口的宽度
#define MS_WINDOW_HEIGHT 480    // 标准窗口的高度

class MatrixLayout;
/**
* @brief
* 类LayoutFactory 矩阵创建工厂
* @param[in]
* @param[in]
*/

typedef struct
{
    int rows;
    int cols;
    int width;
    int height;
}MatrixLayoutAttribute;

typedef struct
{
    int row;
    int col;
    QRectF rect;
}MatrixCellInfo;

class MatrixFactory
{
public:
    static MatrixFactory *Instance();
    static void Release();

    /**
    * @brief
    * produce_layout 产生layout
    * @param[in] rows 行数
    * @param[in] cols 列数
    * @param[in] width 宽度
    * @param[in] height 高度
    * author slf
    */
    QVector<MatrixCellInfo> produce_layout( const int rows, const int cols, const int width, const int height );
private:
    /**
    * @brief
    * is_product_layout 停止组报警
    * @param[in] rows 行数
    * @param[in] cols 列数
    * @param[in] width 宽度
    * @param[in] height 高度
    * author slf
    */
    bool is_product_layout( const int rows, const int cols, const int width, const int height );
private:
    static MatrixFactory* m_factory;
    // 两者key要一致
    QMap<int, MatrixLayoutAttribute>        m_layout_attribute_map;//
    QMap<int, QVector<MatrixCellInfo> >     m_layout_rect_list_map;
};


#endif // LAYOUTFACTORY_H
