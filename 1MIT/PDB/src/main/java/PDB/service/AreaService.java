package PDB.service;
import PDB.dao.AreaDao;
import PDB.entity.PDBArea;
import PDB.entity.PDBDefaultEntity;
import PDB.entity.PDBvehicle;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;

@Service
public class AreaService {

    @Autowired
    private AreaDao areaDao;

    public ResponseEntity<Collection<PDBArea>> getAllAreas() {
        Collection<PDBArea> areas = new ArrayList<PDBArea>();
        try{
            areas = this.areaDao.getAreas();

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(areas);
    }

    public ResponseEntity<Collection<PDBArea>> getParkingSports(Integer id) {
        Collection<PDBArea> areas = null;
        try{
            areas = this.areaDao.getParkingSpots(id);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(areas);
    }

    public ResponseEntity<PDBArea> createArea(PDBDefaultEntity inArea) {
        PDBArea area = null;
        try{
            area = this.areaDao.createArea(inArea);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
            return ResponseEntity.ok().body(area);
    }

    public ResponseEntity<Collection<PDBDefaultEntity>> getDefaultEntities() {
        Collection<PDBDefaultEntity> pdbDefaultEntities = new ArrayList<PDBDefaultEntity>();
        pdbDefaultEntities.add(new PDBDefaultEntity("kruh"));
        pdbDefaultEntities.add(new PDBDefaultEntity("ctverec"));
        pdbDefaultEntities.add(new PDBDefaultEntity("trojuhelnik"));
        pdbDefaultEntities.add(new PDBDefaultEntity("elko"));
        pdbDefaultEntities.add(new PDBDefaultEntity("oval"));
        return ResponseEntity.ok().body(pdbDefaultEntities);
    }

    public ResponseEntity<Collection<Integer>> getEmptySpots() {
        Collection<Integer> emptySpots = new ArrayList<Integer>();
        try{
            emptySpots = this.areaDao.getEmptySpots();

        } catch(SQLException| IOException e) {
            return ResponseEntity.ok(emptySpots);
        }
        return ResponseEntity.ok().body(emptySpots);
    }

    public ResponseEntity<Collection<PDBvehicle>> getVehiclesInAreas(Integer idA, Integer idB) {
        Collection<PDBvehicle> vehicles = new ArrayList<PDBvehicle>();
        try{
            vehicles = this.areaDao.getVehiclesInAreas(idA, idB);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(vehicles);
    }

    public ResponseEntity<PDBArea> updateArea(PDBDefaultEntity pdbDefaultEntity) {
        PDBArea area = null;
        try{
            area = this.areaDao.updateArea(pdbDefaultEntity);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(area);
    }

    public ResponseEntity<Double> getAreaSpace(Integer id) {
        Double area = null;
        try{
            area = this.areaDao.getAreaSpace(id);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(area);
    }

    public ResponseEntity<Double> getAreaDistance(Integer idA, Integer idB) {
        Double distance = null;
        try{
            distance = this.areaDao.getDistanceBetweenAreas(idA, idB);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(distance);
    }

    public ResponseEntity<Double> getAreaLength(Integer id) {
        Double areaLength = null;
        try{
            areaLength = this.areaDao.getLength(id);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(areaLength);
    }

    public ResponseEntity<Double> getAveragePrice(Integer id) {
        Double averagePrice = 0.0;
        try{
            averagePrice = this.areaDao.getAverageCarPrice(id);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().body(averagePrice);
    }

    public ResponseEntity<Void> removeArea(Integer id) {
        try{
            this.areaDao.removeArea(id);

        } catch(SQLException| IOException e) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.noContent().build();
    }
}
