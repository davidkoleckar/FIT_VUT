package PDB.service;


import PDB.dao.UserDao;
import PDB.entity.PDBUser;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import static java.util.Collections.emptyList;

@Service
public class UserDetailsServiceImpl implements UserDetailsService {
    @Autowired
    private UserDao applicationUserRepository;

    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {
        PDBUser applicationUser = applicationUserRepository.findByEmail(email);
        if (applicationUser == null) {
            throw new UsernameNotFoundException(email);
        }
        return new User(applicationUser.getEmail(), applicationUser.getHeslo(), emptyList());
    }
}
