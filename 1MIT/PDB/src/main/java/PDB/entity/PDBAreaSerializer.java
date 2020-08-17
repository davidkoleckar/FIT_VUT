package PDB.entity;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;

import java.io.IOException;
import java.util.Arrays;

public class PDBAreaSerializer extends StdSerializer<PDBArea> {

    public PDBAreaSerializer() {
        this(null);
    }

    public PDBAreaSerializer(Class<PDBArea> t) {
        super(t);
    }

    @Override
    public void serialize(
            PDBArea value, JsonGenerator jgen, SerializerProvider provider)
            throws IOException, JsonProcessingException {

        double ordinates[] =  value.getTvar().getOrdinatesArray();
        if(value.getTypEntity().equals("kruh")){
            PDBAreaPoints center = new PDBAreaPoints();
            PDBAreaPoints bottom = new PDBAreaPoints();
            bottom.setX(ordinates[0]);
            bottom.setY(ordinates[1]);
            PDBAreaPoints top =  new PDBAreaPoints();
            top.setX(ordinates[4]);
            top.setY(ordinates[5]);
            center.setX(bottom.getX());
            center.setY((bottom.getY() + top.getY()) / 2);
            Double r = top.getY() - center.getY();
            double ord[] = {center.getX(), center.getY(), r};
            ordinates = ord;
        } else {
            ordinates =  value.getTvar().getOrdinatesArray();

        }
        jgen.writeStartObject();
        jgen.writeNumberField("id", value.getId());
        jgen.writeStringField("nazev", value.getNazev());
        jgen.writeStringField("typEntity", value.getTypEntity());
        jgen.writeStringField("typ", value.getTyp());
        jgen.writeArrayFieldStart("tvar");
        for(int i=0; i < ordinates.length; i = i + 2){
                    jgen.writeStartObject();
                    jgen.writeNumberField("x", ordinates[i]);
                    if(i+1 < ordinates.length){
                        jgen.writeNumberField("y", ordinates[i+1]);
                    } else {
                        jgen.writeNumberField("y", null);
                    }
            jgen.writeEndObject();
        }
        jgen.writeEndArray();
        jgen.writeEndObject();
    }
}