/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 18:50:58 by pfaria-d          #+#    #+#             */
/*   Updated: 2023/10/24 18:50:59 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.hpp"
#include <iostream>

using namespace std;
int main(int ac, char **av) {
  if (ac != 3) {
    cout << YEL "Usage: ./ircserv <port> <password>" NC << endl;
    return 1;
  }
  (void)av; // A CHANGER
  return 0;
}
