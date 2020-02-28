#ifndef _USER_TYPES
#define _USER_TYPES

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#include "error_handling.h"
#include "util.h"

#include <deque>

#define RANDOM_WAIT_TIME_MIN 2
#define RANDOM_WAIT_TIME_MAX 8

#define PATIENT_DELAYED_START_TIME_MIN 4
#define PATIENT_DELAYED_START_TIME_MAX 11

#define RANDOM_DOCTOR_WAIT_TIME_MIN 1
#define RANDOM_DOCTOR_WAIT_TIME_MAX 3

#define DOCTOR_RAND_MIN_ID 0
#define DOCTOR_RAND_MAX_ID 1023091023

#define MAX_DOCTORS 65536
#define MAX_QUEUE_ITEMS 65536

typedef struct PatientQueueItem
{
	pthread_cond_t* cv;
	pthread_mutex_t* mutex;
	pthread_cond_t* cons_cv;
	pthread_mutex_t* cons_mtx;
	struct Patient* patient;
} PatientQueueItem;

typedef struct Doctor
{
	size_t id;
	pthread_mutex_t* task_mutex;
	pthread_mutex_t* queue_mutex;
	std::deque<PatientQueueItem> patient_queue;
} Doctor;

typedef struct DoctorHandler
{
	size_t doctor_count;
	struct Doctor** doctors;
} DoctorHandler;

typedef struct Patient
{
	pthread_t* thread;
	size_t waiting_time;
	struct DoctorHandler* doctor_handler;
	char scheduled;
	char processed;
} Patient;


Patient* create_patient(DoctorHandler*);
void* patient_thread(void*);
void free_patient(Patient*);

Doctor* create_doctor();
char process_patient(Doctor*);
void free_doctor(Doctor*);

void add_patient_to_queue(Doctor*, Patient*, pthread_cond_t*, pthread_mutex_t*, pthread_cond_t*, pthread_mutex_t*);
PatientQueueItem get_front_patient(Doctor*);
void pop_front_patient(Doctor*);

DoctorHandler* create_doctor_handler();
void add_doctor_to_list(DoctorHandler*, Doctor*);
Doctor* get_random_doctor(DoctorHandler*);
void free_doctor_handler(DoctorHandler*);
#endif