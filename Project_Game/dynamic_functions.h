#include <iostream>
#include <map>


typedef void (*func_t)(int value_one, int value_two, bool target);
typedef std::map<std::string,func_t> func_map_t;
func_map_t func_map;






// If this is the attack function this would be like attackValue, and target Player_one  = true; 
// that is if  this players is Player_one then he was attacking  same just with false if you get attacked.
// tis would be decided by server on matchup whose got the start turn.
void attack(int value_one, int value_two, bool target){
	  std::cout << "DoIt"   << std::endl;

};
void func_test2(int value_one, int value_two, bool target){
	  std::cout << "KUlt"   << std::endl;

};
void init(){
func_map["attack"] = &attack;
func_map["shield"] = &func_test2;
func_map["func1"] = &attack;
}