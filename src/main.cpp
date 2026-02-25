
#include <iostream>

#include <QApplication>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	std::cout<<"Start"<<std::endl;

	MainWindow* window = new MainWindow();
	window->show();

	return app.exec();
}
