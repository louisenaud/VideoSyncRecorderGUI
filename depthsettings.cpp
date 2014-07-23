#include "depthsettings.h"
#include <QtWidgets>

Depthsettings::Depthsettings(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Depthsettings::~Depthsettings()
{

}

void Depthsettings::on_applyButton_clicked() {
	//fps values
	QString fps_txt;
	fps_txt = ui.newFPS->toPlainText();
	bool ok;
	int fps_num = fps_txt.toInt(&ok, 10);

	//ROI values
	QString x_txt, y_txt, width_txt, height_txt;
	x_txt = ui.newXCoord->toPlainText();
	y_txt = ui.newYCoord->toPlainText();
	width_txt = ui.newWidth->toPlainText();
	height_txt = ui.newHeight->toPlainText();
	bool ok2, ok3, ok4, ok5;
	int x_num = x_txt.toInt(&ok2, 10);
	int y_num = y_txt.toInt(&ok3, 10);
	int w_num = width_txt.toInt(&ok4, 10);
	int h_num = height_txt.toInt(&ok5, 10);

	//check to make sure entered fps values are valid
	if (ok == false || fps_num < 0 || fps_num > 162)
	{
		QMessageBox msgBox;
		msgBox.setText("Invalid fps values!");
		msgBox.exec();
	}

	//check to make sure ROI values are valid
	else if (ok2 == false || ok3 == false || ok4 == false || ok5 == false || 
		x_num < 540 || x_num >= 1020 || y_num < 60 || y_num >= 360 ||
		w_num <= 0 || w_num > 480 || h_num <= 0 || h_num > 300)
	{
		QMessageBox msgBox2;
		msgBox2.setText("Invalid ROI values!");
		msgBox2.exec();
	}

	//all values are valid
	else 
	{
		QMessageBox msgBox2;
		msgBox2.setText("New Settings Applied!");
		msgBox2.exec();
	}
}


void Depthsettings::on_okButton_clicked() {
	QString str;
	str = ui.newFPS->toPlainText();
	//emit buttonClick(str);
}