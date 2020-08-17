package PDB.entity;

public class PDBAreaPoints {
    private Double x;
    private Double y;

    public PDBAreaPoints(Double x, Double y) {
        this.x = x;
        this.y = y;
    }

    public PDBAreaPoints() {
    }

    public Double getX() {
        return x;
    }

    public void setX(Double x) {
        this.x = x;
    }

    public Double getY() {
        return y;
    }

    public void setY(Double y) {
        this.y = y;
    }

    @Override
    public String toString() {
        return "x " + getX() + " y " + getY();
    }
}

