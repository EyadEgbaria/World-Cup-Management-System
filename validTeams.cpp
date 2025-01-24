#include "validTeams.h"

validTeams::validTeams(int id, int xp):
        team_id(id),
        xp(xp)
{
}

int validTeams::getID()
{
    return this->team_id;
}

bool operator< (const validTeams& team_a, const validTeams& team_b)
{
    return team_a.team_id < team_b.team_id;
}

bool operator== (const validTeams& team_a, const validTeams& team_b)
{
    return team_a.team_id == team_b.team_id;
}

bool operator!= (const validTeams& team_a, const validTeams& team_b)
{
    return !(team_a == team_b);
}

bool operator<= (const validTeams& team_a, const validTeams& team_b)
{
    return !(team_b < team_a);
}

bool operator>= (const validTeams& team_a, const validTeams& team_b)
{
    return team_b <= team_a;
}

bool operator> (const validTeams& team_a, const validTeams& team_b)
{
    return team_b < team_a;
}

void validTeams::setxp(int xp)
{
    this->xp += xp;
}
int validTeams::getXp()
{
    return xp;
}