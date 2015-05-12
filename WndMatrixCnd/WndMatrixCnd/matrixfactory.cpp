#include "matrixfactory.h"
#include <QDebug>

MatrixFactory* MatrixFactory::m_factory = NULL;


MatrixFactory *MatrixFactory::Instance()
{
    if (m_factory == NULL)  {
        m_factory = new MatrixFactory();
    }
    return m_factory;
}

void MatrixFactory::Release()
{
    if (m_factory)
    {
        delete m_factory;
        m_factory = 0;
    }
}

//产生小矩形
QVector<MatrixCellInfo> MatrixFactory::produce_layout( const int rows, const int cols, const int width, const int height )
{
    QVector<MatrixCellInfo> rect_list;
    if ( rows == 0 || cols == 0 || width == 0 || height == 0 ){
        return rect_list;
    }

    MatrixCellInfo cell = {0};
    float w_uint = ( float )width / rows;
    float h_uint = ( float )height / cols;
    for  (  int i=0; i<rows; i++ )
    {
        for ( int j=0; j<cols; j++ )
        {
            cell.row = i;
            cell.col = j;
            cell.rect.setLeft( w_uint * j );
            cell.rect.setTop( h_uint * i );
            cell.rect.setRight( cell.rect.left( ) + w_uint );
            cell.rect.setBottom( cell.rect.top( ) + h_uint );
            rect_list.push_back( cell );
        }
    }
    MatrixLayoutAttribute attribute;
    attribute.rows = rows;
    attribute.cols = cols;
    attribute.width = width;
    attribute.height = height;

    m_layout_attribute_map[ rows*cols ] = attribute;
    m_layout_rect_list_map[ rows*cols ] = rect_list;

    return rect_list;
}

bool MatrixFactory::is_product_layout( const int rows, const int cols, const int width, const int height )
{
    bool ret = true;
    MatrixLayoutAttribute layout_attribute = m_layout_attribute_map[ rows*cols ];
    if ( layout_attribute.width == width && layout_attribute.height == height ){
        ret = false;
    }
    return ret;
}


