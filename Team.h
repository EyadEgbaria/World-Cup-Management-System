//
// Created by ameer on 12/28/2022.
//

#ifndef UNTITLED1_TEAM_H
#define UNTITLED1_TEAM_H
#include <memory>
#include "PlayerByGoals.h"
#include "PlayerByID.h"
#include "validTeams.h"


using std::shared_ptr;

class Team{
private:
    int team_id;
    int xp;
    int points;
    int numPlayers;
    int numGoalkeeprs;
    bool is_valid;
    int gamesPlayed;
    int id_highest_scorer;
    int cards_highest_scorer;
    int goals_highest_scorer;
    shared_ptr<Tree<PlayerByID>> team_players_by_id_tree;
    shared_ptr<Tree<PlayerByGoals>> team_players_by_goals_tree;
    shared_ptr<validTeams> valid_team;
public:
    Team(int id, int points);
    Team(const Team& team) = default;
    ~Team () = default;

    int getID();
    int get_id_highest_scorer();
    int getxp();
    int get_points();
    shared_ptr<validTeams> getValidTeam();
    bool get_is_valid();
    void set_team_insert(int xp_to_add , bool goalkeeprer , shared_ptr<PlayerByGoals> player_to_add);
    void set_team_remove(int xp_to_decrease , bool goalkeeprer , shared_ptr<PlayerByGoals> highest);
    void set_top_scorer(int id , int cards , int goals);
    shared_ptr<Tree<PlayerByID>> get_team_players_by_id_tree();
    shared_ptr<Tree<PlayerByGoals>> get_team_players_by_goals_tree();
    void changeTopscorerAfterRemove();

    int getGamesPlayed();
    void setxp(int new_xp);
    void addxp(int to_add);
    void set_gamesplayed(int new_games_played);
    void set_points(int new_points);
    int getNumPlayers();
    void set_numPlayers(int num);
    void set_numGoalkeepers(int num);
    int get_num_Goalkeepers();
    void setValidTeam(shared_ptr<validTeams> new_valid);
    void set_invalid(bool b);
    int get_cards_topscorer();
    int get_goals_topscorer();
    void set_by_id(shared_ptr<Tree<PlayerByID>> t);
    void set_by_goals(shared_ptr<Tree<PlayerByGoals>> t);
    void update_top_scorer(shared_ptr<PlayerByGoals> player);
    friend bool operator< (const Team& team_a, const Team& team_b);
    friend bool operator== (const Team& team_a, const Team& team_b);
};

bool operator!= (const Team& team_a, const Team& team_b);
bool operator<= (const Team& team_a, const Team& team_b);
bool operator>= (const Team& team_a, const Team& team_b);
bool operator> (const Team& team_a, const Team& team_b);

#endif //UNTITLED1_TEAM_H
