package Subject;
import Person.*;
import Room.*;

public class Subject {

    Room room;
    Person teacher;
    int noOfStudents;

    Subject(Room room, Person teacher, int noOfStudents) {
        this.room = room;
        this.teacher = teacher;
        this.noOfStudents = noOfStudents;
    }

    public Room getRoom() {
        return room;
    }

    public void setRoom(Room room) {
        this.room = room;
    }

    public Person getTeacher() {
        return teacher;
    }

    public void setTeacher(Person teacher) {
        this.teacher = teacher;
    }

    public int getNoOfStudents() {
        return noOfStudents;
    }

    public void setNoOfStudents(int noOfStudents) {
        this.noOfStudents = noOfStudents;
    }
}
