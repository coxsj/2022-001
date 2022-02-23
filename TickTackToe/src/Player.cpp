#include "Player.h"


void Player::enterPlayerResult(PlayerResult r) {
    results.back() = std::move(r);
}
PlayerResult Player::getLastPlayerResult(){
    //Should only be called after results have been added
    PlayerResult invalid;
    return results.size() > 0 ? results.back() :  invalid;
}
PlayerResult Player::getPlayerResultFromSession(unsigned int sessNum) {
    PlayerResult invalid;
    for (auto r : results)
        if (r.getSessionNum() == sessNum) return r;
    return invalid;
}
bool Player::results_addNewRecord(){
    if (results.size() < cMaxResults) {
        results.push_back(PlayerResult());
        return true;
    }
    return false;
}

