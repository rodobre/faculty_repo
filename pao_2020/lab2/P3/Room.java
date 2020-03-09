package Room;

public class Room {
    String number;
    String floor;
    String type;

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    public String getFloor() {
        return floor;
    }

    public void setFloor(String floor) {
        this.floor = floor;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public Room(String number, String type, String floor) {
        this.number = number;
        this.type = type;
        this.floor = floor;
    }
}
