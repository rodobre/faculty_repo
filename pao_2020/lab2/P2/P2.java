import Person.*;

public class P2 {
    public static void main(String[] args) {
        Person person1 = new Person("Ana", "Maria", "69", 1234567, "student");
        Person person2 = new Person("Buggs", "Bunny", "30", 7654321, "teacher");

        System.out.println(person1.getName() + " " + person1.getSurname() + " " + person1.getAge() + " " +
                person1.getIdentityNumber() + " " + person1.getType());
        System.out.println(person2.getName() + " " + person2.getSurname() + " " + person2.getAge() + " " +
                person2.getIdentityNumber() + " " + person2.getType());
    }
}
