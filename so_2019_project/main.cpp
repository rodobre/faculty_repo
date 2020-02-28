#include "user_classes.h"

#define MAX_TEST_DOCTORS 2u
#define MAX_TEST_PATIENTS 100u

size_t processed_patients = 0u;
pthread_mutex_t* processed_lock = NULL;

void* doctor_process_thread(void* param)
{
	printf("Doctor process thread is online for doctor %p\n", param);
	sleep(get_rand(4, 8));
	Doctor* doctor = (Doctor*)param;
	while(1)
	{
		pthread_mutex_lock(processed_lock);
		if(processed_patients == MAX_TEST_PATIENTS)
		{
			printf("Finished processing patients on this thread.\n");
			pthread_mutex_unlock(processed_lock);
			return NULL;
		}
		pthread_mutex_unlock(processed_lock);

		if(process_patient(doctor) == 1)
		{
			pthread_mutex_lock(processed_lock);
			++ processed_patients;
			pthread_mutex_unlock(processed_lock);
		}
	}
}

int main(int argc, char** argv)
{
	DoctorHandler* doctor_handler = create_doctor_handler();
	Patient** patients = (Patient**)malloc(sizeof(Patient*) * MAX_TEST_PATIENTS);
	pthread_t* doctor_threads = (pthread_t*)malloc(sizeof(pthread_t) * MAX_TEST_DOCTORS);
	processed_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(processed_lock, NULL);

	for(size_t i = 0u; i < MAX_TEST_DOCTORS; ++i)
	{
		Doctor* doctor = create_doctor();
		add_doctor_to_list(doctor_handler, doctor);
		pthread_create(&doctor_threads[i], NULL, doctor_process_thread, doctor);
	}

	for(size_t i = 0u; i < MAX_TEST_PATIENTS; ++i)
		patients[i] = create_patient(doctor_handler);

	for(size_t i = 0u; i < MAX_TEST_DOCTORS; ++i)
		pthread_join(doctor_threads[i], NULL);

	for(size_t i = 0u; i < MAX_TEST_PATIENTS; ++i)
		free_patient(patients[i]);

	SAFE_FREE(patients);
	free_doctor_handler(doctor_handler);
	SAFE_FREE(doctor_threads);
	pthread_mutex_destroy(processed_lock);
	SAFE_FREE(processed_lock);
	return 0;
}