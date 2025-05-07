#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct s_data;

typedef struct s_philo {
    struct s_data   *data;
    pthread_t       t1;
    int             id;
    int             eat_cont;
    int             status;
    int             eating;
    uint64_t        time_to_die;
    pthread_mutex_t lock;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
} t_philo;

typedef struct s_data {
    pthread_t       *tid;
    int             philo_num;
    int             meals_nb;
    int             dead;
    int             finished;
    t_philo         *philos;
    uint64_t        death_time;
    uint64_t        eat_time;
    uint64_t        sleep_time;
    uint64_t        start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t lock;
    pthread_mutex_t write;
} t_data;

// Core functions
void    *philo_routine(void *arg);
void    *monitor_routine(void *arg);
int     check_death(t_data *data);

// Utility functions
void    clear_data(t_data *data);
void    ft_exit(t_data *data);
int     error(char *str, t_data *data);
uint64_t get_time(void);
int     ft_usleep(useconds_t time);

#endif