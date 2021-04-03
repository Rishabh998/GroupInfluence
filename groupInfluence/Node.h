/*
 * Node.h
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	int val;
	float weight;
	int group;
public:
	Node(int val);
	Node(int val,float weight);
	Node(int val,float weight,int group);
};

#endif /* NODE_H_ */
