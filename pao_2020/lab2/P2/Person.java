package Person;

public class Person {
    private String name;
    private String surname;
    private String age;
    private String type;
    private long identityNumber;

    public Person(String name, String surname, String age, long identityNumber, String type) {
        this.name = name;
        this.surname = surname;
        this.age = age;
        this.identityNumber = identityNumber;
        this.type = type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public long getIdentityNumber() {
        return identityNumber;
    }

    public void setIdentityNumber(long identityNumber) {
        this.identityNumber = identityNumber;
    }
}

