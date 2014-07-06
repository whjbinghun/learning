class MyEdit:public QlineEdit{
	Q_OBJECT 
public:
	MyEdit( QWidget * parent, const char * name );
	void enterEvent( QEvent * e );
};	
MyEdit::MyEdit( QWidget * parent, const char * name ) : QLineEdit( parent, name ){
	this->setText("11.11");
}
void MyEdit::enterEvent( QEvent * e ) {
	QString s = this->text();
	emit textChanged ( s );
}
//我现在把textChanged (s)信号连接到一个显示部件里，用QLCDNumber部件。
class MyWindow:public QVBox{
public:
	MyWindow();
	private:
		MyEdit * lineedit;
		QLCDNumber * lcdnum;
};
MyWindow::MyWindow(){
	QHBox* hbox1 = new QHBox(this);
	lineedit = new MyEdit( hbox1, 0 );
	QHBox* hbox2 = new QHBox(this);
	lcdnum = new QLCDNumber(hbox2);
	connect( lineedit, SIGNAL( textChanged ( const QString & ) ), lcdnum, SLOT( display ( const QString & ) ) );
}

//主函数：
int main(int argc, char **argv){
	QApplication a( argc, argv );
	MyWindow w;
	a.setMainWidget( &w );
	w.setGeometry( 500, 200, 200, 150 );
	w.show();
	a.exec();
}