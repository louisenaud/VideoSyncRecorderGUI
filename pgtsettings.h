#ifndef PGTSETTINGS_H
#define PGTSETTINGS_H

#include <QDialog>
#include "ui_pgtsettings.h"

class PGTsettings : public QDialog
{
	Q_OBJECT

public:
	PGTsettings(QWidget *parent = 0);
	~PGTsettings();
	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();
		//void checkValues();

signals:
	//void valueChanged(const QString&);
	//signal that OK button was clicked 
	void buttonClick(const QString& txt);
	//void textChanged();

private:
	Ui::PGTsettings ui;
};

#endif // PGTSETTINGS_H
