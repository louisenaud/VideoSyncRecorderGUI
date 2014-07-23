#ifndef PGFSETTINGS_H
#define PGFSETTINGS_H

#include <QDialog>
#include "ui_pgfsettings.h"

class PGFsettings : public QDialog
{
	Q_OBJECT

public:
	PGFsettings(QWidget *parent = 0);
	~PGFsettings();
	private slots:
		void on_applyButton_clicked();
		void on_okButton_clicked();

signals:
	//signal that OK button was clicked 
	void buttonClick(const QString& txt);

private:
	Ui::PGFsettings ui;
};

#endif // PGFSETTINGS_H
