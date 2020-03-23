#ifndef KIND_H
#define KIND_H

#include <QString>

class Kind
{
 public:
    int getIdKind() const;
    void setIdKind(int );

    QString getName() const;
    void setName(const QString &);

    QString getDescription() const;
    void setDescription(const QString &);

    bool isChecked() const;
    void setChecked(bool value);

    bool operator <  ( const Kind  kind ) const;
    bool operator == ( const Kind  kind )const;
   
private:
    int idKind;
    QString name;
    QString description;
    bool checked;
};

#endif
