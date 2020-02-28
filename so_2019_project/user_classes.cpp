#include "user_classes.h"

Patient* create_patient(DoctorHandler* doctor_handler)
{
	Patient* patient = (Patient*)(malloc(sizeof(Patient)));
	ERROR_CHECK_CND(patient, NULL, "Could not allocate memory for patient");

	pthread_t* patient_thread_t = (pthread_t*)(malloc(sizeof(pthread_t)));
	ERROR_CHECK_CND(patient_thread_t, NULL, "Could not alloacte memory for patient thread");

	patient->waiting_time = 0u;
	patient->doctor_handler = doctor_handler;
	patient->thread = patient_thread_t;
	patient->scheduled = 0;
	patient->processed = 0;

	ERROR_CHECK(pthread_create(patient_thread_t, NULL, patient_thread, patient), "Could not create patient thread");
	return patient;
}

void* patient_thread(void* ptr)
{
	printf("Patient thread (%p) is now online\n", ptr);
	struct timeval before_task, after_task, after_cons;
	sleep(get_rand(PATIENT_DELAYED_START_TIME_MIN, PATIENT_DELAYED_START_TIME_MAX));

	Patient* patient = (Patient*)ptr;

	pthread_mutex_t* patient_task_mutex = (pthread_mutex_t*)(malloc(sizeof(pthread_mutex_t)));
	pthread_cond_t* patient_task_cvar = (pthread_cond_t*)(malloc(sizeof(pthread_cond_t)));
	pthread_mutex_t* patient_cons_mutex = (pthread_mutex_t*)(malloc(sizeof(pthread_mutex_t)));
	pthread_cond_t* patient_cons_cvar = (pthread_cond_t*)(malloc(sizeof(pthread_cond_t)));
	ERROR_CHECK_CND(patient_task_mutex, NULL, "Could not allocate memory for patient task mutex!");
	ERROR_CHECK_CND(patient_task_cvar, NULL, "Could not allocate memory for patient task cvar!");
	ERROR_CHECK_CND(patient_cons_mutex, NULL, "Could not allocate memory for patient cons mutex!");
	ERROR_CHECK_CND(patient_cons_cvar, NULL, "Could not allocate memory for patient cons cvar!");

	pthread_mutex_init(patient_task_mutex, NULL);
	pthread_mutex_init(patient_cons_mutex, NULL);
	pthread_cond_init(patient_task_cvar, NULL);
	pthread_cond_init(patient_cons_cvar, NULL);

	Doctor* random_doctor = get_random_doctor(patient->doctor_handler);
	gettimeofday(&before_task, NULL);

	pthread_mutex_lock(random_doctor->queue_mutex);
	add_patient_to_queue(random_doctor, patient, patient_task_cvar, patient_task_mutex, patient_cons_cvar, patient_cons_mutex);
	pthread_mutex_unlock(random_doctor->queue_mutex);
	
	pthread_mutex_lock(patient_task_mutex);
	while(!patient->scheduled)
		pthread_cond_wait(patient_task_cvar, patient_task_mutex);
	pthread_mutex_unlock(patient_task_mutex);

	gettimeofday(&after_task, NULL);
	printf("Patient %p is now in the doctor's office.\n", ptr);
	
	pthread_mutex_lock(patient_cons_mutex);
	while(!patient->processed)
		pthread_cond_wait(patient_cons_cvar, patient_cons_mutex);
	pthread_mutex_unlock(patient_cons_mutex);

	printf("Patient %p finished being processed.\n", ptr);
	gettimeofday(&after_cons, NULL);

	printf("It took us %u to get inside the doctor's office\n"
		   "It took us %u to get checked.\n", get_time_difference(before_task, after_task)/1000u, get_time_difference(after_task, after_cons)/1000u);

	ERROR_CHECK(pthread_mutex_destroy(patient_task_mutex), "Cannot destroy patient task mutex!");
	ERROR_CHECK(pthread_mutex_destroy(patient_cons_mutex), "Cannot destroy patient cons mutex!");
	ERROR_CHECK(pthread_cond_destroy(patient_task_cvar), "Cannot destroy patient task cvar!");
	ERROR_CHECK(pthread_cond_destroy(patient_cons_cvar), "Cannot destroy patient cons cvar!");
	SAFE_FREE(patient_task_mutex);
	SAFE_FREE(patient_task_cvar);
	SAFE_FREE(patient_cons_mutex);
	SAFE_FREE(patient_cons_cvar);
	return NULL;
}

void free_patient(Patient* patient)
{
	ERROR_CHECK(pthread_join(*patient->thread, NULL), "Could not join pthread in free_patient");
	SAFE_FREE(patient->thread);
	SAFE_FREE(patient);
}

Doctor* create_doctor()
{
	Doctor* doctor = (Doctor*)malloc(sizeof(Doctor));
	pthread_mutex_t* task_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_t* queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	std::deque<PatientQueueItem> patient_queue;
	ERROR_CHECK_CND(doctor, NULL, "Could not allocate memory for the doctor!");
	ERROR_CHECK_CND(task_mutex, NULL, "Could not allocate memory for doctor task mutex!");
	ERROR_CHECK_CND(queue_mutex, NULL, "Could not allocate memory for doctor queue cvar!");

	ERROR_CHECK(pthread_mutex_init(task_mutex, NULL), "Cannot initialize doctor task mutex!");
	ERROR_CHECK(pthread_mutex_init(queue_mutex, NULL), "Cannot initialize doctor task mutex!");

	doctor->id = get_rand(DOCTOR_RAND_MIN_ID, DOCTOR_RAND_MAX_ID);
	doctor->task_mutex = task_mutex;
	doctor->queue_mutex = queue_mutex;
	doctor->patient_queue = std::move(patient_queue);
	return doctor;
}

char process_patient(Doctor* doctor)
{
	pthread_mutex_lock(doctor->task_mutex);
	pthread_mutex_lock(doctor->queue_mutex);
	if(doctor->patient_queue.empty())
	{
		pthread_mutex_unlock(doctor->task_mutex);
		pthread_mutex_unlock(doctor->queue_mutex);
		return 0;
	}

	PatientQueueItem queue_item = doctor->patient_queue.front();
	doctor->patient_queue.pop_front();

	printf("Processing patient... preparing to signal the condition variable\n");
	
	pthread_mutex_lock(queue_item.mutex);
	queue_item.patient->scheduled = 1;
	pthread_mutex_unlock(queue_item.mutex);
	pthread_cond_signal(queue_item.cv);

	sleep(get_rand(RANDOM_DOCTOR_WAIT_TIME_MIN, RANDOM_DOCTOR_WAIT_TIME_MAX));
	
	pthread_mutex_lock(queue_item.cons_mtx);
	queue_item.patient->processed = 1;
	pthread_mutex_unlock(queue_item.cons_mtx);	
	pthread_cond_signal(queue_item.cons_cv);

	printf("Finished processing patient %p on doctor %p\n", queue_item.patient, doctor);
	pthread_mutex_unlock(doctor->task_mutex);
	pthread_mutex_unlock(doctor->queue_mutex);
	return 1;
}

void free_doctor(Doctor* doctor)
{
	ERROR_CHECK(pthread_mutex_destroy(doctor->task_mutex), "Cannot initialize doctor task mutex!");
	ERROR_CHECK(pthread_mutex_destroy(doctor->queue_mutex), "Cannot destroy doctor queue mutex!");
	SAFE_FREE(doctor->task_mutex);
	SAFE_FREE(doctor->queue_mutex);
	SAFE_FREE(doctor);
}

void add_patient_to_queue(Doctor* doctor, Patient* patient, pthread_cond_t* task_cv, pthread_mutex_t* task_mtx,
										pthread_cond_t* cons_cv, pthread_mutex_t* cons_mtx)
{
	printf("Added patient %p to queue of doctor %p\n", patient, doctor);
	PatientQueueItem item;
	item.patient = patient;
	item.cv = task_cv;
	item.mutex = task_mtx;
	item.cons_cv = cons_cv;
	item.cons_mtx = cons_mtx;
	doctor->patient_queue.push_back(item);
}

DoctorHandler* create_doctor_handler()
{
	DoctorHandler* doctor_handler = (DoctorHandler*)malloc(sizeof(DoctorHandler));
	Doctor** doctors = (Doctor**)malloc(sizeof(Doctor*) * MAX_DOCTORS);
	ERROR_CHECK_CND(doctor_handler, NULL, "Could not allocate memory for the doctor handler!");
	ERROR_CHECK_CND(doctors, NULL, "Could not allocate memory for the doctors!");

	doctor_handler->doctor_count = 0u;

	for(size_t i = 0u; i < MAX_DOCTORS; ++i)
		doctors[i] = create_doctor();

	doctor_handler->doctors = doctors;
	return doctor_handler;
}

void add_doctor_to_list(DoctorHandler* doctor_handler, Doctor* doctor)
{
	printf("Added doctor %p to list\n", doctor);
	if(doctor_handler->doctor_count + 1 >= MAX_DOCTORS)
		return;

	free_doctor(doctor_handler->doctors[doctor_handler->doctor_count]);
	doctor_handler->doctors[doctor_handler->doctor_count] = doctor;
	++ doctor_handler->doctor_count;
}

Doctor* get_random_doctor(DoctorHandler* doctor_handler)
{
	size_t rand_idx = get_rand(0, doctor_handler->doctor_count);
	return doctor_handler->doctors[rand_idx];
}

void free_doctor_handler(DoctorHandler* doctor_handler)
{
	for(size_t i = 0u; i < MAX_DOCTORS; ++i)
		free_doctor(doctor_handler->doctors[i]);

	SAFE_FREE(doctor_handler->doctors);
	SAFE_FREE(doctor_handler);
}