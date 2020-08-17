package PDB.entity;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;
import oracle.sql.DATE;

import java.io.IOException;
import java.util.Objects;

/**
 * Trida pro serializaci datumu
 */
public class DATECustomSerializer extends StdSerializer<DATE> {

    public DATECustomSerializer() {
        this(null);
    }

    public DATECustomSerializer(Class t) {
        super(t);
    }

    @Override
    public void serialize (DATE value, JsonGenerator gen, SerializerProvider arg2)
            throws IOException, JsonProcessingException {
        gen.writeString(value.toString().equals("") ? null : value.toString());
    }
}