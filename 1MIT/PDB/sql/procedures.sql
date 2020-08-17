CREATE OR REPLACE PROCEDURE rotate
    (vozidlo_id IN NUMBER)
IS
    image ORDSYS.ORDImage;
BEGIN
    SELECT photo INTO image FROM obrazky
    WHERE id = vozidlo_id FOR UPDATE;
    image.process('rotate=90');
    COMMIT;
END;
