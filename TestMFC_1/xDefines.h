// File Name: xDefines.h
#pragma once

#define SAFE_DELETE_PTR(ptr) {if(ptr!=nullptr){delete [] ptr; ptr=nullptr;}}
#define SAFE_DELETE_OBJ(ptr) {if(ptr!=nullptr){delete ptr; ptr=nullptr;}}

