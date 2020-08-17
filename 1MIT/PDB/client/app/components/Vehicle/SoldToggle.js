import React from 'react';
import styled from 'styled-components';

import { Toggle } from 'office-ui-fabric-react/lib/Toggle';

const StyledToggle = styled(Toggle)`

  display: inline-flex;
  align-item: center;
  width: 16rem;
  margin-bottom: 0;
  padding: 0 1rem;
  
  button {
    margin: 0 1rem;
  }

  label {
    padding: 0;
  }

  .ms-Toggle-innerContainer {
    align-items: center;
  }

`;

export default StyledToggle;