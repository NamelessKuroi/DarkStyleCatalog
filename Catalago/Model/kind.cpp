#include "kind.h"

int Kind::getIdKind() const
{
    return idKind;
}

void Kind::setIdKind(int value)
{
    idKind = value;
}

QString Kind::getName() const
{
    return name;
}

void Kind::setName(const QString &value)
{
    name = value;
}

QString Kind::getDescription() const
{
    return description;
}

void Kind::setDescription(const QString &value)
{
    description = value;
}


bool Kind::isChecked() const
{
    return checked;
}

void Kind::setChecked(bool value)
{
    checked = value;
}

bool Kind::operator < ( const Kind  kind ) const
{
    return name < kind.getName();
}

bool Kind::operator ==( const Kind  kind )const
{
    return idKind == kind.getIdKind();
}



