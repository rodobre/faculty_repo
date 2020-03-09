package Subject;
import Person.*;
import Room.*;

public class P4 {
    public static void main(String[] args) {
        Person person1 = new Person("Ana", "Maria", "69", 1234567, "student");
        Person person2 = new Person("Buggs", "Bunny", "30", 7654321, "teacher");
        Room room1 = new Room("12A", "biology", "3");
        Room room2 = new Room("12B", "tech", "7");

        Subject subject1 = new Subject(room1, person1, 30);
        Subject subject2 = new Subject(room2, person2, 30);

        System.out.println(subject1.teacher.getName() + " " + subject1.teacher.getSurname() + " " + subject1.teacher.getAge() + " " +
                subject1.teacher.getIdentityNumber() + " " + subject1.teacher.getType() + " that lives at " + subject1.room.getNumber() + " " +
                subject1.room.getType() + " " + subject1.room.getFloor() + " has " + subject1.noOfStudents + " students.");
        System.out.println(subject2.teacher.getName() + " " + subject2.teacher.getSurname() + " " + subject2.teacher.getAge() + " " +
                subject2.teacher.getIdentityNumber() + " " + subject2.teacher.getType() + " that lives at " + subject2.room.getNumber() + " " +
                subject2.room.getType() + " " + subject2.room.getFloor() + " has " + subject2.noOfStudents + " students.");
    }
}
