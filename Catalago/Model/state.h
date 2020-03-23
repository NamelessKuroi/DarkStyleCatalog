#ifndef STATE_H
#define STATE_H

#include <QString>

class State
{
 public:

    int getIdState() const;
    void setIdState(int value);

    QString getName() const;
    void setName(const QString &);

    QString getDescription() const;
    void setDescription(const QString &);

    bool isChecked() const;
    void setChecked(bool value);

    bool operator < ( const State  state ) const;
    bool operator ==( const State  state ) const;
   


private:
    int idState;
    QString name;
    QString description;
    bool checked;
};

#endif
