package PDB.service;

import PDB.dao.VehicleDao;
import PDB.entity.PDBUserPrice;
import PDB.entity.PDBUserVehicle;
import PDB.entity.PDBvehicle;
import oracle.sql.DATE;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.context.config.ResourceNotFoundException;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Collection;

@Service
public class VehicleService {

    @Autowired
    private VehicleDao vehicleDao;

    public ResponseEntity<Collection<PDBvehicle>> getAllVehicles(Boolean sold){
        Collection<PDBvehicle> pdBvehicles = null;
        try {
            pdBvehicles = this.vehicleDao.getVehicles(sold);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdBvehicles);
    }

    public ResponseEntity<PDBvehicle> getVehicleById(Long id) {
        PDBvehicle pdbVehicle = null;
        try {
            pdbVehicle = this.vehicleDao.getVehicle(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdbVehicle);
    }

    public ResponseEntity<PDBvehicle> updateVehicle(PDBvehicle vehicle) {
        PDBvehicle pdbVehicle = null;
        try {
            pdbVehicle = this.vehicleDao.updateVehicle(vehicle);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdbVehicle);
    }

    public ResponseEntity<PDBvehicle> createVehicle(PDBvehicle vehicle) {
        PDBvehicle pdbVehicle = null;
        try {
            pdbVehicle = this.vehicleDao.createVehicle(vehicle);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdbVehicle);
    }

    public ResponseEntity<Void> removeVehicle(Long id) {
        PDBvehicle pdbVehicle = null;
        try {
            this.vehicleDao.deleteVehicle(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.noContent().build();
    }

    public ResponseEntity<Collection<PDBUserVehicle>> getEditedVehicles(DATE from, DATE to) {
        Collection<PDBUserVehicle> pdbUserVehicle = null;
        try {
            pdbUserVehicle = this.vehicleDao.getEditedVehicles(from, to);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdbUserVehicle);
    }

    public ResponseEntity<Collection<PDBUserPrice>> getVehiclePrices(Long id) {
        Collection<PDBUserPrice> pdbuserPrices = null;
        try {
            pdbuserPrices = this.vehicleDao.getVehiclePrices(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(pdbuserPrices);
    }
}
