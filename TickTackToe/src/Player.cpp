#include "Player.h"


void Player::enterResult(GameResult r) {
    results.back() = std::move(r);
}
GameResult Player::getLastResult(){
    //Should only be called after results have been added
    GameResult invalid;
    return results.size() > 0 ? results.back() :  invalid;
}
GameResult Player::getResultFromSession(unsigned int sessNum) {
    GameResult invalid;
    for (auto r : results)
        if (r.getSessionNum() == sessNum) return r;
    return invalid;
}
bool Player::results_addNewRecord(){
    if (results.size() < cMaxResults) {
        results.push_back(GameResult());
        return true;
    }
    return false;
}

