/*
 * reinforcement_learning.h
 *
 *  Created on: 2025/08/03
 *      Author: k-abe
 */

#ifndef REINFORCEMENT_LEARNING_H
#define REINFORCEMENT_LEARNING_H

#define Q_TABLE_STATES_NUM	(4)	// 状態の数
#define Q_TABLE_ACTIONS_NUM	(3)	// 行動の数

typedef float (*QtablePtr)[5];

extern int rl_get_state(void);
extern int rl_get_reward(void);
extern void rl_move(int action);
extern void rl_init_Qtable(void);
extern float rl_max_Qval(int state, int num_actions, QtablePtr Qtable);
extern QtablePtr rl_get_Qtable_address(void);
extern int rl_select_action(int state, int num_actions, QtablePtr Qtable);
extern void rl_init_epsilon_greedy(void);
extern int rl_epsilon_greedy(int epsilon, int state, int num_action, QtablePtr Qtable);

#endif /* REINFORCEMENT_LEARNING_H */
