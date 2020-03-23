#include "state.h"

int State::getIdState() const
{
    return idState;
}

void State::setIdState(int value)
{
    idState = value;
}


QString State::getName() const
{
    return name;
}

void State::setName(const QString &value)
{
    name = value;
}

QString State::getDescription() const
{
    return description;
}

void State::setDescription(const QString &value)
{
    description = value;
}

bool State::isChecked() const
{
    return checked;
}

void State::setChecked(bool value)
{
    checked = value;
}


 bool State::operator < ( const State  state ) const
 {
    return name < state.getName();
 }

bool  State::operator ==( const State  state )const
{
    return idState == state.getIdState();
}

