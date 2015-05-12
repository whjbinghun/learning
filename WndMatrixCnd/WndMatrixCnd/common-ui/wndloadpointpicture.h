#ifndef WNDLOADPOINTPICTURE_H
#define WNDLOADPOINTPICTURE_H

#include <QDialog>

namespace Ui {
class WndLoadPointPicture;
}

class WndLoadPointPicture : public QDialog
{
    Q_OBJECT

public:
    explicit WndLoadPointPicture(QWidget *parent = 0);
    ~WndLoadPointPicture();

    void keyPressEvent( QKeyEvent *event  );
    void keyReleaseEvent( QKeyEvent *event );

    void set_caption( const QString & str_caption );
    void open_ir_vi_image( QString str_ir_path, QString str_vi_path );
    void delete_all_ana();

    void set_palette_enable( bool b_enable );
private slots:
    void slot_palette_change_clr( int n_index );
    void slot_palette_auto();
    void slot_palette_change_range( float f_level, float f_level_span );
private:
    void init_palette();
    void update_palette();
private:
    Ui::WndLoadPointPicture *ui;
};

#endif // WNDLOADPOINTPICTURE_H
