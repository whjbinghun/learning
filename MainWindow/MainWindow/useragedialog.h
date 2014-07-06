#ifndef USERAGEDIALOG_H
#define USERAGEDIALOG_H

class UserAgeDialog{

};
// in dialog:
void UserAgeDialog::accept()
{
    emit userAgeChanged(newAge); // newAge is an int
    QDialog::accept();
}
#endif // USERAGEDIALOG_H
