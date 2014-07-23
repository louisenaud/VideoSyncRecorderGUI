#include "videosyncrecordergui.h"
#include <QtWidgets/QApplication>
#include <omp.h>
int main(int argc, char *argv[])
{
	qRegisterMetaType<vector<QImage>>("vector<QImage>");
	QApplication a(argc, argv);
	VideoSyncRecorderGUI w;
	//w.setAttribute(Qt::WA_DeleteOnClose, true);
	w.show();
	return a.exec();
	
}
