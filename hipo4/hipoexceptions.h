/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hipoexceptions.h
 * Author: gavalian
 *
 * Created on April 11, 2017, 2:06 PM
 */

#ifndef HIPOEXCEPTIONS_H
#define HIPOEXCEPTIONS_H

#include <exception>

class HipoWrongFile : public std::exception {
  virtual const char* what() const throw(){
    return "exception: wrong Hipo File format.";
  }
};

class HipoRecordError : public std::exception {
  virtual const char* what() const throw(){
    return "exception: error parsing record";
  }
};


#endif /* HIPOEXCEPTIONS_H */

