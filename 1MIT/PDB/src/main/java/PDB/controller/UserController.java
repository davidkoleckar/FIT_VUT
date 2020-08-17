package PDB.controller;

import PDB.entity.PDBUser;
import PDB.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

/**
 * Trida pro zpracovani requestu uzivatele
 */
@RestController
@RequestMapping("/users")
public class UserController {

    @Autowired
    private UserService userService;

    @Autowired
    private BCryptPasswordEncoder bCryptPasswordEncoder;

    /**
     * Ziskani vsech uzivatelu
     * @return
     */
    @RequestMapping(method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBUser>> getAllUsers(){
        Authentication auth = SecurityContextHolder.getContext().getAuthentication();
        return userService.getAllUsers();
    }

    /**
     * Vytvoreni uzivatele
     * @param user
     * @return
     */
    @RequestMapping(method = RequestMethod.POST)
    public ResponseEntity<PDBUser> createUser(@RequestBody PDBUser user){
        return userService.createUser(user);
    }

    /**
     * Ziksani uzivatele na zaklade id
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.GET)
    public ResponseEntity<PDBUser> getUserById(@PathVariable("id") long id){
        return userService.getUserById(id);
    }

    /**
     * Uprava uzivatele na zaklade id
     * @param user
     * @return
     */
    @RequestMapping(method = RequestMethod.PUT)
    public ResponseEntity<PDBUser> editUser(@RequestBody PDBUser user){
        return userService.updateUser(user);
    }

    /**
     * Smazani uzivatele na zaklade id
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.DELETE)
    public ResponseEntity<PDBUser> deleteUserById(@PathVariable("id") long id){
        return userService.removeUserById(id);
    }

    /**
     * Registrace uzivatele
     * @param user
     */
    @PostMapping("/sign-up")
    public void signUp(@RequestBody PDBUser user) {
        this.userService.createUser(user);
    }
}
