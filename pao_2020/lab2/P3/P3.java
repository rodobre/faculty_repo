import Room.*;

public class P3 {
    public static void main(String[] args) {
        Room room1 = new Room("12A", "biology", "3");
        Room room2 = new Room("12B", "tech", "7");
        System.out.println(room1.getNumber() + " " + room1.getType() + " " + room1.getFloor());
        System.out.println(room2.getNumber() + " " + room2.getType() + " " + room2.getFloor());
    }
}
