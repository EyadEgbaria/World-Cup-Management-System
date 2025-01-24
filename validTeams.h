//
// Created by ameer on 12/28/2022.
//

#ifndef UNTITLED1_VALIDTEAMS_H
#define UNTITLED1_VALIDTEAMS_H



#include "tree.h"
#include <memory>

using std::shared_ptr;


class validTeams{
private:
    int team_id;
    int xp;
public:
    validTeams(int id, int xp);
    validTeams(const validTeams& team) = default;
    ~validTeams () = default;

    void setxp(int xp);
    int getID();
    int getXp();


    friend bool operator< (const validTeams& team_a, const validTeams& team_b);
    friend bool operator== (const validTeams& team_a, const validTeams& team_b);
};

bool operator!= (const validTeams& team_a, const validTeams& team_b);
bool operator<= (const validTeams& team_a, const validTeams& team_b);
bool operator>= (const validTeams& team_a, const validTeams& team_b);
bool operator> (const validTeams& team_a, const validTeams& team_b);

#endif //UNTITLED1_VALIDTEAMS_H
