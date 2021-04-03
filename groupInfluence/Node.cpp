/*
 * Node.cpp
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#include "Node.h"
#include "String"


Node::Node(int v)
{
	val=v;
	weight=1.0;
	group=-1;
}

Node::Node(int v,float w)
{
	val=v;
	weight=w;
	group=-1;
}
Node::Node(int v,float w,int g)
{
	val=v;
	weight=w;
	group=g;
}



