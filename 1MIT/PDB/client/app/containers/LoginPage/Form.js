import React from 'react';
import PropTypes from 'prop-types';

import { DefaultButton } from 'office-ui-fabric-react/lib/Button';
import { TextField } from 'office-ui-fabric-react/lib/TextField';

const Form = ({ onSubmit, onChange, email, heslo }) => (
  <form onSubmit={onSubmit}>
    <div className="form-group">
      <TextField
        label="Email"
        onChanged={(value) => onChange({ email: value })}
        value={email}
        id="loginForm-email"
        placeholder="spravce@autobazaru.cz"
        underlined
        required
      />
    </div>
    <div className="form-group">
      <TextField
        label="Heslo"
        type="password"
        onChanged={(value) => onChange({ heslo: value })}
        value={heslo}
        placeholder="Secret"
        underlined
        required
      />
    </div>
    <DefaultButton className="mt-3" type="submit">
      Přihlásit
    </DefaultButton>
  </form>
);

Form.propTypes = {
  onSubmit: PropTypes.func.isRequired,
  onChange: PropTypes.func.isRequired,
  email: PropTypes.string.isRequired,
  heslo: PropTypes.string.isRequired,
};

export default Form;
