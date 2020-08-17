import styled from 'styled-components';

import { TextField } from 'office-ui-fabric-react/lib/TextField';

const StyledSlider = styled(TextField)`

  .ms-TextField-fieldGroup > div {
    order: ${(props) => props.addonAfter ? 1 : 0};
  }

`;

export default StyledSlider;