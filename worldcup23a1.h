//
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
//
// Recommended TAB size to view this file: 8.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "Team.h"
#include <memory>
#include <cmath>
#include <exception>
using std::shared_ptr;

class world_cup_t {
private:
    //
    // Here you may add anything you want
    //
    int id_top_scorer;
    int goals_of_highest_scorer;
    int cards_of_highest_scorer;
    int num_all_players;
    shared_ptr<Tree<PlayerByID>> all_players_by_id_tree;
    shared_ptr<Tree<PlayerByGoals>> all_players_by_goals_tree;
    shared_ptr<Tree<Team>> all_teams;
    shared_ptr<Tree<validTeams>> valid_teams;

    int best_player(shared_ptr<PlayerByGoals> p1 ,shared_ptr<PlayerByGoals> p2 ,shared_ptr<PlayerByGoals> current );
    void update_top_scorer(shared_ptr<PlayerByGoals> player);
    void change_highest_scorer_before_remove(shared_ptr<PlayerByGoals> player);
    void change_highest_scorer_before_remove_in_the_team(shared_ptr<Team> team_node , shared_ptr<PlayerByGoals> player);
    void updatePreAndSuc(shared_ptr<TreeNode<PlayerByGoals>> root , shared_ptr<PlayerByGoals> key);
    int max_top_scorer(int id1 , int goal1 , int cards1 , int id2 , int goal2 , int cards2);
    int knockout_aux(int* id_array , int* xp_array , int length);
public:
    // <DO-NOT-MODIFY> {

    world_cup_t();
    virtual ~world_cup_t();

    StatusType add_team(int teamId, int points);

    StatusType remove_team(int teamId);

    StatusType add_player(int playerId, int teamId, int gamesPlayed,
                          int goals, int cards, bool goalKeeper);

    StatusType remove_player(int playerId);

    StatusType update_player_stats(int playerId, int gamesPlayed,
                                   int scoredGoals, int cardsReceived);

    StatusType play_match(int teamId1, int teamId2);

    output_t<int> get_num_played_games(int playerId);

    output_t<int> get_team_points(int teamId);

    StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

    output_t<int> get_top_scorer(int teamId);

    output_t<int> get_all_players_count(int teamId);

    StatusType get_all_players(int teamId, int *const output);

    output_t<int> get_closest_player(int playerId, int teamId);

    output_t<int> knockout_winner(int minTeamId, int maxTeamId);

    // } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
